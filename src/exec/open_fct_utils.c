/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fct_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:22:39 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 13:25:14 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_existing_file(const char *file)
{
	if (access(file, W_OK) == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", file);
		return (1);
	}
	return (0);
}

int	check_parent_dir(const char *file)
{
	char	*parent_dir;
	char	*last_slash;
	int		ret;

	parent_dir = ft_strdup(file);
	if (!parent_dir)
		return (1);
	last_slash = ft_strrchr(parent_dir, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(parent_dir, W_OK) == -1)
			ret = 1;
		else
			ret = 0;
	}
	else
		ret = 0;
	free(parent_dir);
	if (ret)
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", file);
	return (ret);
}
