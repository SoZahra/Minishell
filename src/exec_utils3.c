/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:29:55 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:30:08 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_path(const char *dir, const char *file)
{
	size_t	len_dir;
	size_t	len_file;
	char	*full_path;

	len_file = ft_strlen(file);
	len_dir = ft_strlen(dir);
	full_path = malloc(len_dir + len_file + 2);
	if (!full_path)
	{
		perror("malloc");
		return (NULL);
	}
	strcpy(full_path, dir);
	if (full_path[len_dir - 1] != '/')
		strcat(full_path, "/");
	strcat(full_path, file);
	return (full_path);
}

int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (1);
	return (0);
}
