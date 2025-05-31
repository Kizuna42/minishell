/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:19:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 20:19:20 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_invalid_var(char **result, char *var_name, int dollar_pos, int *i)
{
	if (!var_name || (ft_strlen(var_name) == 0 && (*result)[*i] != '?'
		&& (*result)[*i] != '$'))
	{
		if (var_name)
			free(var_name);
		*i = dollar_pos + 1;
		return (1);
	}
	return (0);
}

void	update_position(int *i, char *var_value, int dollar_pos)
{
	if (var_value)
		*i = dollar_pos + ft_strlen(var_value);
	else
		*i = dollar_pos;
}
